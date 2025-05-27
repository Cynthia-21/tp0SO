import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import domain.Categoria;
import domain.Coleccion;
import domain.Etiqueta;
import domain.criteriosPertenencia.CriterioDePertenencia;
import domain.filtrosDeBusqueda.Filtrador;
import domain.Hecho;
import domain.criteriosPertenencia.CriterioCategoria;
import domain.criteriosPertenencia.ICriterioDePertenencia;
import domain.criteriosPertenencia.CriterioFecha;
import domain.filtrosDeBusqueda.Filtro;
import domain.fuentes.FuenteEstatica;
import domain.origen.Origen;
import domain.ubicacion.Coordenadas;
import domain.ubicacion.Ubicacion;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class TestEscenario1 {
  private Coleccion coleccion;
  private Hecho hecho1, hecho2, hecho3, hecho4, hecho5;
  private Categoria caidaAeronave, accidenteMaquinaria, accidentePasoNivel, derrumbreEnObra;

  @BeforeEach
  public void init() {
    this.coleccion = new Coleccion("Colección prueba");
    this.coleccion.setDescripcion("Esto es una prueba");

    this.caidaAeronave = new Categoria("Categoria Aeronave");
    this.accidenteMaquinaria = new Categoria("Accidente con maquinaria industrial");
    this.accidentePasoNivel = new Categoria("Accidente en paso nivel");
    this.derrumbreEnObra = new Categoria("Derrumbe en obra en construcción");

    this.hecho1 = new Hecho("Caída de aeronave impacta en Olavarría");
    this.hecho1.agregarCategoria(this.caidaAeronave);
    this.hecho1.setDescripcion("Grave caída de aeronave ocurrió en las inmediaciones de Olavarría, Buenos Aires. El incidente provocó pánico entre los residentes locales. Voluntarios de diversas organizaciones se han sumado a las tareas de auxilio.");
    this.hecho1.setUbicacion(new Ubicacion(new Coordenadas(-36.868375, -60.343297)));
    this.hecho1.setFechaAcontecimiento(LocalDate.of(2001, 11, 29));
    this.hecho1.setOrigen(Origen.MANUAL);

    this.hecho2 = new Hecho("Serio incidente: Accidente con maquinaria industrial en Chos Malal, Neuquén");
    this.hecho2.setDescripcion("Un grave accidente con maquinaria industrial se registró en Chos Malal, Neuquén. El incidente dejó a varios sectores sin comunicación. Voluntarios de diversas organizaciones se han sumado a las tareas de auxilio");
    this.hecho2.agregarCategoria(this.accidenteMaquinaria);
    this.hecho2.setUbicacion(new Ubicacion(new Coordenadas(-37.345571, -70.241485)));
    this.hecho2.setFechaAcontecimiento(LocalDate.of(2001, 8, 16));
    this.hecho2.setOrigen(Origen.MANUAL);

    this.hecho3 = new Hecho("Caída de aeronave impacta en Venado Tuerto, Santa Fe");
    this.hecho3.setDescripcion("Grave caída de aeronave ocurrió en las inmediaciones de Venado Tuerto, Santa Fe. El incidente destruyó viviendas y dejó a familias evacuadas. Autoridades nacionales se han puesto a disposición para brindar asistencia");
    this.hecho3.agregarCategoria(this.caidaAeronave);
    this.hecho3.setUbicacion(new Ubicacion(new Coordenadas(-33.768051, -61.921032)));
    this.hecho3.setFechaAcontecimiento(LocalDate.of(2008, 8, 8));
    this.hecho3.setOrigen(Origen.MANUAL);

    this.hecho4 = new Hecho("Accidente en paso a nivel deja múltiples daños en Pehuajó, Buenos Aires");
    this.hecho4.setDescripcion("Grave accidente en paso a nivel ocurrió en las inmediaciones de Pehuajó, Buenos Aires. El incidente generó preocupación entre las autoridades provinciales. El Ministerio de Desarrollo Social está brindando apoyo a los damnificados.");
    this.hecho4.agregarCategoria(this.accidentePasoNivel);
    this.hecho4.setUbicacion(new Ubicacion(new Coordenadas(-35.855811, -61.940589)));
    this.hecho4.setFechaAcontecimiento(LocalDate.of(2020, 1, 27));
    this.hecho4.setOrigen(Origen.MANUAL);

    this.hecho5 = new Hecho("Devastador Derrumbe en obra en construcción afecta a Presidencia Roque Sáenz Peña");
    this.hecho5.setDescripcion("Un grave derrumbe en obra en construcción se registró en Presidencia Roque Sáenz Peña, Chaco. El incidente generó preocupación entre las autoridades provinciales. El intendente local se ha trasladado al lugar para supervisar las operaciones.");
    this.hecho5.agregarCategoria(this.derrumbreEnObra);
    this.hecho5.setUbicacion(new Ubicacion(new Coordenadas(-26.78008, -60.45872)));
    this.hecho5.setFechaAcontecimiento(LocalDate.of(2016, 6, 4));
    this.hecho5.setOrigen(Origen.MANUAL);

    List<Hecho> hechos = List.of(this.hecho1, this.hecho2, this.hecho3, this.hecho4, this.hecho5);

    FuenteEstatica fuenteEstatica = mock(FuenteEstatica.class);
    this.coleccion.agregarFuente(fuenteEstatica);
    when(fuenteEstatica.importarHechos()).thenReturn(hechos);

    this.coleccion.agregarHechosDeFuente();

}

  @Test
  @DisplayName("Validar que se puedan obtener los hechos a partir de la colección")
  public void test1() {

    //Un hecho Puntual
    System.out.println("Primer Hecho: ");
    System.out.println(this.coleccion.getHechos().get(0));

    System.out.println("Todos los hechos: ");
    System.out.println(this.coleccion);
    System.out.println(this.coleccion.getHechos());

    assertEquals(5, this.coleccion.getHechos().size());
  }

  @Test
  @DisplayName("Agregar criterio de fecha (1/1/2000 - 1/1/2010) y recalcular hechos (solo quedan 3)")
  public void test2() {

    System.out.println("COLECCION ORIGINAL");
    System.out.println(this.coleccion);

    //agrego criterio de pertenencia
    ICriterioDePertenencia criterioFechas = new CriterioFecha(
        LocalDate.of(2000, 1, 1),
        LocalDate.of(2010, 1, 1)
    );
    CriterioDePertenencia criterioDelHecho = new CriterioDePertenencia();
    criterioDelHecho.agregarCriterio(criterioFechas);
    this.coleccion.setCriterioDePertenencia(criterioDelHecho);

    this.coleccion.revalidarPorCriterio();

    System.out.println("Hehcos entre 1/1/2000 y 1/1/210");
    System.out.println(this.coleccion);
    System.out.println(this.coleccion.getHechos());

    assertEquals(3, this.coleccion.getHechos().size());
    assertTrue(this.coleccion.getHechos().contains(this.hecho1));
    assertTrue(this.coleccion.getHechos().contains(this.hecho2));
    assertTrue(this.coleccion.getHechos().contains(this.hecho3));
    assertFalse(this.coleccion.getHechos().contains(this.hecho4));
    assertFalse(this.coleccion.getHechos().contains(this.hecho5));
  }

  @Test
  @DisplayName("Sumar otro criterio y recalcular los hechos")
  public void test3() {
    System.out.println("COLECCION ORIGINAL");
    System.out.println(this.coleccion);

    //agrego criterio de pertenencia
    ICriterioDePertenencia criterioFechas = new CriterioFecha(
        LocalDate.of(2000, 1, 1),
        LocalDate.of(2010, 1, 1)
    );

    ICriterioDePertenencia criterioCategoria = new CriterioCategoria(this.caidaAeronave);

    CriterioDePertenencia criterioTotal = new CriterioDePertenencia();
    criterioTotal.agregarCriterio(criterioCategoria);
    criterioTotal.agregarCriterio(criterioFechas);

    this.coleccion.setCriterioDePertenencia(criterioTotal);
    this.coleccion.revalidarPorCriterio();

    System.out.println("Hechos entre 1/1/2000 y 1/1/210 + categoria 'caida  aeronave'");
    System.out.println(this.coleccion);
    System.out.println(this.coleccion.getHechos());

    assertEquals(2, this.coleccion.getHechos().size());
    assertTrue(this.coleccion.getHechos().contains(this.hecho1));
    assertFalse(this.coleccion.getHechos().contains(this.hecho2));
    assertTrue(this.coleccion.getHechos().contains(this.hecho3));
    assertFalse(this.coleccion.getHechos().contains(this.hecho4));
    assertFalse(this.coleccion.getHechos().contains(this.hecho5));

  }

  @Test
  @DisplayName("Ningun hecho cumple con filtros categoria 'caida aeronave' y titulo 'un titulo'")
  public void test4() {
    System.out.println("COLECCION ORIGINAL");
    System.out.println(this.coleccion);

    Filtro filtro = new Filtro();
    filtro.setTitulo("titulo");
    filtro.agregarCategoria(this.caidaAeronave);

    Filtrador filtrador = new Filtrador();
    List<Hecho> lista = new ArrayList<>();
    lista = filtrador.filtrar(this.coleccion, filtro);

    System.out.println("COLECCION FILTRADA");
    System.out.println(lista);

    assertEquals(5, this.coleccion.getHechos().size());
    assertEquals(0, lista.size());
    assertFalse(filtro.cumpleFiltro(this.hecho1));
  }

  @Test
  @DisplayName("Etiqueto un hecho")
  public void test5() {
    System.out.println("HECHO ORIGINAL");
    System.out.println(this.hecho1);

    Etiqueta etiquetaOlavarria = new Etiqueta("Olavarria");
    Etiqueta etiquetaGrave = new Etiqueta("Grave");

    hecho1.agregarEtiqueta(etiquetaOlavarria);
    hecho1.agregarEtiqueta(etiquetaGrave);
    hecho1.agregarEtiqueta(etiquetaGrave); //no la agrega dos veces

    System.out.println("HECHO CON ETIQUETAS");
    System.out.println(this.hecho1);

    assertEquals(2, this.hecho1.getEtiquetas().size());
    assertTrue(this.hecho1.getEtiquetas().contains(etiquetaOlavarria));
  }

  }
