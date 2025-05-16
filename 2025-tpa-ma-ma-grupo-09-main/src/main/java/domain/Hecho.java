package domain;

import domain.contenidoMultimedia.ContenidoMultimedia;
import domain.fuentes.estrategiasFuenteEstatica.Archivo;
import domain.origen.Origen;
import domain.ubicacion.Ubicacion;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import lombok.Getter;
import lombok.Setter;



@Getter
@Setter
public class Hecho {
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private ContenidoMultimedia contenidoMultimedia;
  private Ubicacion ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDate fechaDeCarga;
  private Origen origen;
  private Boolean estaEliminado;
  private Contribuyente contribuyente;
  private List<Etiqueta> etiquetas;
  private Archivo archivoOrigen;

  public Hecho(String titulo) {
    this.titulo = titulo;
    this.descripcion = "";
    this.categorias = new ArrayList<>();
    this.contenidoMultimedia = null;
    this.ubicacion = null;
    this.fechaAcontecimiento = null;
    this.fechaDeCarga = LocalDate.now();
    this.origen = null;
    this.estaEliminado = false;
    this.contribuyente = null; //Atributo pensado para usar a futuro en una entrega con fuentes dinámicas
    this.etiquetas = new ArrayList<>();
    this.archivoOrigen = null;
  }


  public boolean contieneTexto(String texto) {
    return titulo.toLowerCase().contains(texto.toLowerCase())
        || descripcion.toLowerCase().contains(texto.toLowerCase());
  }


  public void agregarCategoria(Categoria categoria) {
    if (!this.categorias.contains(categoria)) {
      this.categorias.add(categoria);
    }
  }

  public void agregarEtiqueta(Etiqueta etiqueta) {
    if (!this.etiquetas.contains(etiqueta)) {
      this.etiquetas.add(etiqueta);
    }
  }


  //Para un print
  @Override
  public String toString() {
    return "Hecho { \n\t"
        + "titulo = '" + titulo + "\n\t"
        + "categorias = " + categorias.stream().map(Categoria::getNombre).toList() + "\n\t"
        + "ubicacion = " + ubicacion + "\n\t"
        + "fecha = " + fechaAcontecimiento + "\n\t"
        + "origen = " + origen + "\n\t"
        + "etiquetas = " + etiquetas + "\n}"
        + "}\n";
  }
}
