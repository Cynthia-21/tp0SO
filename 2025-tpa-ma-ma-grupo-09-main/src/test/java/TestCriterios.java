import static org.junit.jupiter.api.Assertions.*;

import domain.Categoria;
import domain.Hecho;
import domain.criteriosPertenencia.CriterioCategoria;
import domain.criteriosPertenencia.CriterioDePertenencia;
import domain.criteriosPertenencia.ICriterioDePertenencia;
import domain.criteriosPertenencia.CriterioFecha;
import domain.criteriosPertenencia.CriterioUbicacion;
import domain.origen.Origen;
import domain.ubicacion.Coordenadas;
import domain.ubicacion.Ubicacion;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import java.time.LocalDate;

public class TestCriterios {
  private Hecho hecho;
  private Categoria categoriaDesastre;
  private Categoria categoriaIncendio;

  @BeforeEach
  public void init() {
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(-10.3214, 54.1233));
    LocalDate fecha = LocalDate.of(2025, 4, 17);
    this.hecho = new Hecho("Desastre");
    this.hecho.setUbicacion(ubicacion);
    this.hecho.setFechaAcontecimiento(fecha);
    this.hecho.setOrigen(Origen.DATASET);

    this.categoriaDesastre = new Categoria("Desastre");
    this.categoriaIncendio = new Categoria("Incendio");
  }

  @Test
  @DisplayName("El hecho 'Desastre' cumple con el criterio de categoría")
  public void cumpleCriterioCategoria() {
    this.hecho.agregarCategoria(this.categoriaDesastre);
    ICriterioDePertenencia criterio = new CriterioCategoria(this.categoriaDesastre);
    assertTrue(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' NO cumple con el criterio categoría")
  public void noCumpleCriterioCategoria() {

    this.hecho.agregarCategoria(this.categoriaDesastre);
    ICriterioDePertenencia criterio = new CriterioCategoria(this.categoriaIncendio);
    assertFalse(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' cumple uno de los criterios")
  public void cumpleAlgunCriterio() {
    this.hecho.agregarCategoria(this.categoriaDesastre);
    this.hecho.agregarCategoria(this.categoriaIncendio);
    ICriterioDePertenencia criterio = new CriterioCategoria(this.categoriaIncendio);
    assertTrue(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' cumple criterio de fecha")
  public void cumpleCritertioFecha() {
    this.hecho.setFechaAcontecimiento(LocalDate.of(2020, 4, 17));
    ICriterioDePertenencia criterio = new CriterioFecha(LocalDate.of(2015, 1, 1), LocalDate.of(2025, 1, 1));
    assertTrue(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' no cumple criterio de fecha")
  public void noCumpleCritertioFecha() {
    this.hecho.setFechaAcontecimiento(LocalDate.of(2020, 4, 17));
    ICriterioDePertenencia criterio = new CriterioFecha(LocalDate.of(2005, 1, 1), LocalDate.of(2015, 1, 1));
    assertFalse(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' cumple criterio ubicacion")
  public void cumpleCriterioUbicacion() {
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(-10.3214, 54.1233));
    ICriterioDePertenencia criterio = new CriterioUbicacion(ubicacion);
    assertTrue(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' no cumple criterio ubicacion")
  public void noCumpleCriterioUbicacion() {
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(60.3214, -122.1233));
    ICriterioDePertenencia criterio = new CriterioUbicacion(ubicacion);
    assertFalse(criterio.cumpleCriterio(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' cumple criterio compuesto")
  public void cumpleCriterioCompuesto() {
    this.hecho.agregarCategoria(this.categoriaDesastre);
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(-10.3214, 54.1233));
    ICriterioDePertenencia criterioCategoria = new CriterioCategoria(this.categoriaDesastre);
    ICriterioDePertenencia criterioFecha = new CriterioFecha(LocalDate.of(2016, 1, 1), LocalDate.of(2026, 1, 1));
    ICriterioDePertenencia criterioUbicacion = new CriterioUbicacion(ubicacion);
    CriterioDePertenencia criterioTotal = new CriterioDePertenencia();
    criterioTotal.agregarCriterio(criterioCategoria);
    criterioTotal.agregarCriterio(criterioFecha);
    criterioTotal.agregarCriterio(criterioUbicacion);
    assertTrue(criterioTotal.cumpleCriterios(this.hecho));
  }

  @Test
  @DisplayName("El hecho 'desastre' no cumple criterio compuesto")
  public void noCumpleCriterioCompuesto() {
    this.hecho.agregarCategoria(this.categoriaDesastre);
    Ubicacion ubicacion = new Ubicacion(new Coordenadas(10.3214, -54.1233));
    ICriterioDePertenencia criterioCategoria = new CriterioCategoria(this.categoriaIncendio);
    ICriterioDePertenencia criterioFecha = new CriterioFecha(LocalDate.of(2006, 1, 1), LocalDate.of(2016, 1, 1));
    ICriterioDePertenencia criterioUbicacion = new CriterioUbicacion(ubicacion);
    CriterioDePertenencia criterioTotal = new CriterioDePertenencia();
    criterioTotal.agregarCriterio(criterioCategoria);
    criterioTotal.agregarCriterio(criterioFecha);
    criterioTotal.agregarCriterio(criterioUbicacion);
    assertFalse(criterioTotal.cumpleCriterios(this.hecho));
  }
}
