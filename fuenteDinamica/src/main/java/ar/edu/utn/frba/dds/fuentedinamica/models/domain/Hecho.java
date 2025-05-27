package ar.edu.utn.frba.dds.fuentedinamica.models.domain;

import ar.edu.utn.frba.dds.fuentedinamica.models.domain.contenidoMultimedia.ContenidoMultimedia;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.origen.Origen;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.ubicacion.Ubicacion;
import ar.edu.utn.frba.dds.fuentedinamica.models.domain.Contribuyente;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.chrono.ChronoLocalDateTime;
import java.util.ArrayList;
import java.util.List;



@Getter
@Setter
public class Hecho {
  private Long id;
  private String titulo;
  private String descripcion;
  private List<Categoria> categorias;
  private ContenidoMultimedia contenidoMultimedia;
  private Ubicacion ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
  private Origen origen;
  private Boolean estaEliminado;
  private Contribuyente contribuyente;
  private List<Etiqueta> etiquetas;
  private LocalDate fechaCreacion;

  public Hecho() {
    this.id = id;
    this.titulo = titulo;
    this.categorias = new ArrayList<>();
    this.fechaDeCarga = LocalDateTime.now();
    this.estaEliminado = false;
    this.etiquetas = new ArrayList<>();
  }

  public boolean puedeSerEditado() {
    return contribuyente != null
        && contribuyente.isEstaRegistrado()
        && LocalDateTime.now().isBefore(fechaDeCarga.plusDays(7));
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
